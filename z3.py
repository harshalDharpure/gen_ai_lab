import sys
from collections import defaultdict
import copy

def read_input(filename):
    N = 0
    K = 0
    assignments = {}
    deps = defaultdict(list)

    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("%"):
                continue

            parts = line.split()

            if parts[0] == "N":
                N = int(parts[1])

            elif parts[0] == "K":
                K = int(parts[1])

            elif parts[0] == "A":
                aid = int(parts[1])
                prompts = int(parts[2])
                dependencies = list(map(int, parts[3:-1]))

                assignments[aid] = prompts
                deps[aid] = dependencies

    return N, K, assignments, deps


def ready_tasks(assignments, deps, completed):
    r = []
    for a in assignments:
        if a not in completed:
            ok = True
            for d in deps[a]:
                if d not in completed:
                    ok = False
                    break
            if ok:
                r.append(a)
    return r


def backtrack(day, N, K, assignments, deps, completed, schedule, m):

    if len(completed) == len(assignments):
        print("VALID SCHEDULE\n")
        for d in schedule:
            print("Day", d, schedule[d])
        print()
        return

    if day > m:
        return

    tasks = ready_tasks(assignments, deps, completed)

    def assign(student, remaining_prompts, available_tasks, done_today):

        if student == N or not available_tasks:
            new_completed = completed | set(done_today)
            new_schedule = copy.deepcopy(schedule)
            new_schedule[day] = done_today
            backtrack(day + 1, N, K, assignments, deps, new_completed, new_schedule, m)
            return

        for t in available_tasks:
            p = assignments[t]

            if p <= remaining_prompts[student]:
                remaining_prompts[student] -= p
                next_tasks = [x for x in available_tasks if x != t]
                assign(student, remaining_prompts, next_tasks, done_today + [(student+1, t)])
                remaining_prompts[student] += p

        assign(student + 1, remaining_prompts, available_tasks, done_today)

    assign(0, [K]*N, tasks, [])


def main():

    if len(sys.argv) < 3:
        print("Usage: python program.py input.txt days")
        return

    filename = sys.argv[1]
    m = int(sys.argv[2])

    N, K, assignments, deps = read_input(filename)

    backtrack(1, N, K, assignments, deps, set(), {}, m)


if __name__ == "__main__":
    main()
