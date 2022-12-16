with open('../day16/input.txt') as f:
    s = f.read()[:-1].split('\n')

    
def parse(line):
    parts = line.split(' ')
    valve = parts[1]
    flow_rate = int(line[line.index('=')+1:line.index(';')])
    if 'valves ' in line:
        leads_to = line.split('valves ')[1].split(', ')
    else:
        leads_to = line.split('valve ')[1].split(', ')
    return valve,flow_rate,leads_to


def construct_paths(start,leads_to,poi):
    distances = {start:0}
    queue = {start}
    while len(queue) > 0:
        valve = queue.pop()
        d = distances[valve]
        for v in leads_to[valve]:
            if v not in distances or d+1 < distances[v]:
                distances[v] = d+1
                queue.add(v)
    return {k:v+1 for k,v in distances.items() if k in poi and v != 0}


valves = [parse(line) for line in s]
flow_rates = {v[0]:v[1] for v in valves if v[1] != 0}
leads_to = {v[0]:v[2] for v in valves}
distances = {valve:construct_paths(valve,leads_to,flow_rates) for valve in flow_rates}
distances['AA'] = construct_paths('AA',leads_to,flow_rates)


def path_value(visited,total_time,distances,flow_rates):
    value = 0
    time = total_time
    for i in range(len(visited)-1):
        time -= distances[visited[i]][visited[i+1]]
        value += time*flow_rates[visited[i+1]]
    return value


def store(visited,current,value,cache):
    v = visited.copy()
    v.sort()
    v = tuple(v)
    cache[(v,current)] = value
    

def retrieve(visited,current,cache):
    v = visited.copy()
    v.sort()
    v = tuple(v)
    return cache.get((v,current))


def check_cache(visited,current,value,cache,total_time,distances,flow_rates):
    optimal_value = retrieve(visited,current,cache)
    if optimal_value is None or optimal_value < value:
        store(visited,current,value,cache)
    else:
        return optimal_value
    return None


def explore(current,visited,value,remaining_time,total_time,distances,flow_rates,cache):
    cache_result = check_cache(visited,current,value,cache,total_time,distances,flow_rates)
    if cache_result is not None:
        return cache_result
    subpath_values = [value]
    options = [valve for valve in distances[current] if valve not in visited and remaining_time-distances[current][valve] >= 0]
    for valve in options:
        subpath_remaining = remaining_time-distances[current][valve]
        subpath = explore(valve,
                          visited+[valve],
                          path_value(visited+[valve],total_time,distances,flow_rates),
                          subpath_remaining,
                          total_time,
                          distances,
                          flow_rates,
                          cache)
        subpath_values.append(subpath)
    return max(subpath_values)


cache = {}
print(explore('AA',['AA'],0,30,30,distances,flow_rates,cache))


cache = {}
explore('AA',['AA'],0,26,26,distances,flow_rates,cache)

optimal_path_values = {}
for c in cache:
    key = c[0][1:]
    value = cache[c]
    if (key not in optimal_path_values or value > optimal_path_values[key]) and value != 0:
        optimal_path_values[key] = value


def best_pair_for(path,optimal_path_values):
    visited = set(path)
    options = {k:v for k,v in optimal_path_values.items() if len(visited.intersection(set(k))) == 0}
    if len(options) == 0:
        return None
    return max(options,key=options.get)

        
optimal_pairs = {path:best_pair_for(path,optimal_path_values) for path in optimal_path_values}
optimal_pairs = {k:v for k,v in optimal_pairs.items() if v is not None}

print(max([optimal_path_values[k] + optimal_path_values[v] for k,v in optimal_pairs.items()]))
