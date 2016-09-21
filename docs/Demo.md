Demo of Sylva Language
---

```
module PrintNameSupport                 # module is a bunch of functions, static or instance

  require static func name              # `require` keyword declares a function included classes must implemented

  static func printName                 # implements a static func
    system.println name                 # the parentheses can be removed when invoke a function with no arguments
  end

  require func name

  func printName
    system.println name
  end

end

class Animal

  include PrintNameSupport              # include a module is typically add static and instance functions and check existences of requireed functions

  static var name = "animal"

  var name = "noname"                   # static/instance variables can only be accessed via -> operator, . operator will use auto generated or manually created getter/setter

  func name
    return self->name                   # this getter will be automatically added
  end

  func name=(name)
    self->name = name                   # this setter will be automatically added
  end

  func init                             # instance method init is special, it will be called after Class() method to init a instance
  end

  func method1(x)
    if x
    else
    end

    unless x
    else
    end

    while condition
    end

    for var x in array
    end
  end

  func method2(x, anotherArg: y, thirdArg: z)
  end

  func method3(x, y, z)
  end

end

class Deer : Animal
end

```

Sylva - C native binding
---

```
native class NativeClass
end
```
