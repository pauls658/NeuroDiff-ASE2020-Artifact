import tabulate, sys
buf = open(sys.argv[1], "r", encoding="utf-8").read()
blocks = buf.split("./delta_network_test ")
# prop -> time
times = {}
verified_props = {}
total_props = {}
for block in blocks[1:]:
    lines = block.strip().split("\n")

    prop = int(lines[0].split(" ")[0])
    verified = len(list(filter(lambda l: "No adv!" in l, lines))) > 0
    time = list(filter(lambda l: "time:" in l, lines))
    time = 1800.0 if not time else float(time[0].split(":")[1].strip())

    if prop not in times: 
        times[prop] = 0.0
        verified_props[prop] = 0
        total_props[prop] = 0
    times[prop] += time
    verified_props[prop] += int(verified)
    total_props[prop] += 1

times[6] = times[16] + times[26]
verified_props[6] = verified_props[16] + verified_props[26]
total_props[6] = total_props[16] + total_props[26]

headers = ["Property", "Proved", "Undet", "Time (s)"]
rows = []
for prop in sorted(times.keys()):
    if prop == 16 or prop == 26: continue
    rows.append([prop, verified_props[prop], total_props[prop]-verified_props[prop], times[prop]])

print(tabulate.tabulate(rows, headers, tablefmt="github"))
