class foo:
    i = 12
    def bar(self):
        return 42

    def aze(self):
        return "my string"


class plop(foo):
    __privateMember = "internal"
    
    def bar2(self):
        return 43

