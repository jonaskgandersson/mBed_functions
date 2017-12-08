From 
[John Bode](https://stackoverflow.com/a/10759352)

Start with your declaration for f1:
```
int (*f1)(float);
```
You want f2 to be a pointer to a function returning f1, so substitute f1 in the declaration above with the declaration for f2:
```
int (*      f1     )(float);
            |
      +-----+-----+
      |           |
      v           v
int (*(*f2)(double))(float);
```
The declaration reads as
```

        f2                   -- f2
       *f2                   -- is a pointer
      (*f2)(      )          -- to a function
      (*f2)(double)          --   taking a double parameter
     *(*f2)(double)          --   returning a pointer
    (*(*f2)(double))(     )  --   to a function
    (*(*f2)(double))(float)  --     taking a float parameter
int (*(*f2)(double))(float)  --     returning int
```
You repeat the process for f3:
```
int (*(*    f2    )(double))(float);
            |
        +---+----+
        |        |
        v        v
int (*(*(*f3)(int))(double))(float);
```
which reads as
```
          f3                           -- f3
         *f3                           -- is a pointer
        (*f3)(   )                     -- to a function
        (*f3)(int)                     --   taking an int parameter
       *(*f3)(int)                     --   returning a pointer
      (*(*f3)(int))(      )            --   to a function
      (*(*f3)(int))(double)            --     taking a double parameter
     *(*(*f3)(int))(double)            --     returning a pointer
    (*(*(*f3)(int))(double))(     )    --     to a function
    (*(*(*f3)(int))(double))(float)    --       taking a float parameter
int (*(*(*f3)(int))(double))(float);   --       returning int
```
