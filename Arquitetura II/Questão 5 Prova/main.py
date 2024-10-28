import itertools

class Expression:
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def evaluate(self, a, b):
        if self.op == '&':
            return a and b
        elif self.op == '|':
            return a or b
        else:
            assert False

def truth_table(expr):
    if isinstance(expr, str):
        expr = Expression(expr, None, None)

    variables = set()
    stack = [expr]

    while stack:
        node = stack.pop()

        if isinstance(node.left, str):
            variables.add(node.left)
            node.left = Expression(node.op, variables.pop(), None)

        if isinstance(node.right, str):
            variables.add(node.right)
            node.right = Expression(node.op, variables.pop(), None)

        if node.left is not None:
            stack.append(node.left)
        if node.right is not None:
            stack.append(node.right)

    header = ' '.join(sorted(variables))
    print(f'{header}\n{("-" * len(header))}')

    for values in itertools.product((True, False), repeat=len(variables)):
        row = ' '.join(str(value) for value in values)
        result = expr.evaluate(*values)
        print(f'{row}  {result}')

# Example usage:
truth_table('A & (B | C)')