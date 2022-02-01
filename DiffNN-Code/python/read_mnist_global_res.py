import tabulate, sys
buf = open(sys.argv[1], "r", encoding="utf-8").read()
eps = float(sys.argv[2])
blocks = buf.split("./delta_network_test ")

verified_props = {}
total_props = {}
for block in blocks[1:]:
    lines = block.strip().split("\n")

    prop = int(lines[0].strip().split(" ")[5])
    delta_int = list(filter(lambda l: "delta interval" in l, lines))[0]
    delta_int = eval(delta_int.split(":")[1])
    verified = delta_int[0] > -eps and delta_int[1] < eps

    if prop not in verified_props: 
        verified_props[prop] = 0
        total_props[prop] = 0

    verified_props[prop] += int(verified)
    total_props[prop] += 1

headers = ["Perturbation", "Proved", "Undet"]
rows = []
for prop in sorted(verified_props.keys()):
    rows.append([prop, verified_props[prop], total_props[prop]-verified_props[prop]])

print(tabulate.tabulate(rows, headers, tablefmt="github"))
