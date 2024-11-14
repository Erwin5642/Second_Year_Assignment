tape = list("jfaijqpijfipqp")
headPosition = 5

print(f"\r\033[{33}m{tape[:headPosition]}\033[0m", end='')
print(f"\033[{34}m{tape[headPosition:headPosition + 1]}\033[0m", end='')
print(f"\033[{33}m{tape[headPosition + 1:]}\033[0m")
print(f"\r\rOi")