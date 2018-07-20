# producerjson

Producer JSON smart contract for EOS.

# Usage

### View the table

```
cleos get table producerjson producerjson producerjson
```

### Add to the table

```
cleos push action producerjson set '{"owner":"your_account", "json": "your_json"}' -p your_account@active
```

**Example**:
```
cleos push action producerjson set '{"owner":"teamgreymass", "json": "'`printf %q $(cat bp.json)`'"}' -p teamgreymass@active
```

### Remove from the table

```
cleos push action producerjson del '{"owner":"your_account"}' -p your_account@active
```


---

# How to build
```
./build.sh
```
or
```
eosiocpp -g producerjson.abi producerjson.cpp && eosiocpp -o producerjson.wast producerjson.cpp
```

# How to deploy

```
cleos set contract producerjson producerjson -p producerjson@active
```
