import csv
import re
import json

enemies = []

def num(x):
    return int(x.replace(',', ''))

with open('../raw/bcu_en/EnemyName.txt') as file:
    for line in file:
        line = line.strip()

        unit_id = int(line[:3])

        while len(enemies) <= unit_id:
            enemies.append([])

        enemies[unit_id] = {'name': line[4:].strip()}

with open('../raw/battlecats_db/enemies.csv') as csvfile:
    table = [row for row in csv.reader(csvfile)][1:]

    for row in table:
        unit_id = int(row[0][:3]) - 2

        if unit_id < len(enemies): # Ignore stuff not in EN or unreleased
            enemy = enemies[unit_id]

            TRAITS = {
                '白い': 'traitless',
                '赤い': 'red',
                '黒い': 'black',
                '浮いてる': 'floating',
                'メタルな': 'metal',
                '天使': 'angel',
                'エイリアン': 'alien',
                'ゾンビ': 'zombie',
                '古代種': 'relic',
                '悪魔': 'aku',
                '超生命体': 'behemoth',
                '超獣': 'behemoth', # Not sure what the deal with this is?
                '超賢者': 'sage',
                '魔女': 'witch',
                '使徒': 'eva_angel',
                '無属性な敵': 'typeless'
            }
            traits = []
            trait_lines = [line.strip() for line in row[1].splitlines()]
            for line in trait_lines:
                for key, value in TRAITS.items():
                    if key in line:
                        traits.append(value)
            enemy["traits"] = traits

            enemy["health"] = num(row[3])
            enemy["knockbacks"] = num(row[4])
            enemy["speed"] = num(row[5])
            enemy["attack"] = num(row[6])
            enemy["area"] = bool(row[8] == '範囲')
            enemy["attack_frequency"] = num(row[9]) if row[9] != '-' else -1
            enemy["foreswing"] = num(row[10])
            enemy["range"] = num(row[11])
            enemy["value"] = num(row[12])

            # This is the annoying part
            desc = row[13]
            lines = [line.strip() for line in desc.splitlines()]

            # Multi-hit
            # Example: 3連続攻撃 605 150 2,270（11F 15F 35F)
            def extract_multihit(text):
                pattern = r'(\d)連続攻撃((?: \d+)+)[（(]((?:\d+F ?)+)[）)]'
                match = re.search(pattern, text)
                if match:
                    n = int(match.group(1))
                    damage = [int(num) for num in match.group(2).strip().split()[:n]]
                    frames = [int(num.replace('F', '')) for num in match.group(3).split()[:n]]
                    return damage, frames
                return None

            mh = extract_multihit(desc)
            if mh:
                enemy["hit_damages"] = mh[0]
                enemy["hit_frames"] = mh[1]
            else:
                enemy['hit_damages'] = [enemy['attack']]
                enemy['hit_frames'] = [enemy['foreswing']]

            for line in lines:
                # Basic stuff part of many abilities
                match_rate = re.search(r'(\d+)％の確率で', line)
                rate = int(match_rate.group(1)) if match_rate else None

                match_duration = re.search(r'(\d+)F', line)
                duration = int(match_duration.group(1)) if match_duration else None

                match_level = re.search(r'Lv(\d+)', line)
                level = int(match_level.group(1)) if match_level else None

                # Crit
                CRITICAL = 'クリティカル'
                if CRITICAL in line:
                    enemy['crit_rate'] = rate

                match_multiplier = re.search(r'（与ダメ x(\d)）', line)
                multiplier = int(match_multiplier.group(1)) if match_multiplier else None

                # Savage
                SAVAGE = '渾身の一撃'
                if SAVAGE in line:
                    enemy['savage'] = {'rate': rate, 'multiplier': multiplier}

                # Strengthen
                match_strengthen = re.search(r'体力(\d+)％以下で攻撃力上昇', line)
                if match_strengthen:
                    enemy['strengthen'] = {'health_percent': int(match_strengthen.group(1)), 'multiplier': multiplier}

                # Survive
                SURVIVE = '1度だけ生き残る'
                if SURVIVE in line:
                    enemy['survive_rate'] = rate

                # Long-distance/omnistrike
                LONG_DIST = '遠方範囲'
                if LONG_DIST in line:
                    intervals = [(num(match[0]), num(match[1])) for match in re.findall(r'(-?\d+)～(\d+)', line)]
                    if len(intervals) == 1:
                        enemy['effective_l'] = intervals[0][0]
                        enemy['effective_r'] = intervals[0][1]
                    else:
                        enemy['hit_effective_l'] = []
                        enemy['hit_effective_r'] = []
                        for l, r in intervals:
                            enemy['hit_effective_l'].append(l)
                            enemy['hit_effective_r'].append(r)

                # ...
                MINI = '小'

                # Waves
                WAVE = '波動'
                if level and WAVE in line:
                    enemy['miniwave' if MINI in line else 'wave'] = {'rate': rate, 'level': level}

                # Surges
                SURGE = '烈波'
                if level and SURGE in line:
                    match_surge = re.search(r'(\d+)F 射程 (\d+)～(\d+)', line)
                    enemy['minisurge' if MINI in line else 'surge'] = {
                        'rate': rate,
                        'duration': int(match_surge.group(1)),
                        'spawn_l': int(match_surge.group(2)),
                        'spawn_r': int(match_surge.group(3))
                        }

                # Conjure
                match_conjure = re.search(r'召喚（精霊：No\.(\d+)）', line)
                if match_conjure:
                    enemy['conjure'] = int(match_conjure.group(1)) - 1

                # Immunity to various effects, not trait specific
                IMMUNE = '無効'
                if line[:2] == IMMUNE:
                    EFFECTS = {
                        '古代の呪い': 'curse',
                        '止める': 'freeze',
                        'ふっとばす': 'knockback',
                        '烈波': 'surge',
                        '遅くする': 'slow',
                        '毒撃': 'toxic',
                        'ワープ': 'warp',
                        '波動': 'wave',
                        '攻撃力低下': 'weaken'
                    }
                    enemy['immunities'] = []
                    for key, value in EFFECTS.items():
                        if key in line:
                            enemy['immunities'].append(value)

                # Effects with only proc rate
                PROC_ONLY = {
                    'ふっとばす': 'knockback',
                    '攻撃無効': 'dodge'
                }
                for key, value in PROC_ONLY.items():
                    if key in line and rate:
                        enemy[value] = {'rate': rate}

                # Debuffs with proc rate and duration
                TIMED = {
                    '動きを遅くする': 'slow',
                    '動きを止める': 'freeze',
                    'で呪い': 'curse'
                }
                for key, value in TIMED.items():
                    if key in line and rate and duration:
                        enemy[value] = {'rate': rate, 'duration': duration}

                # Only weaken has a % effect value
                weaken = re.search(r'攻撃力(\d+)％に低下', line)
                if weaken:
                    enemy['weaken'] = {'rate': rate, 'duration': duration, 'effect': int(weaken.group(1))}


with open('../parsed/enemies.json', 'w') as fp:
    json.dump(enemies, fp, indent=4)

print('Processing completed!')
print(len(enemies), "enemies")