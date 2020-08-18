# Operation transport protocol

## 1. Introduction

The OPTP is a protocol for transferring operations between multiple nodes concurrently. Nodes mustn't be a physical machines, although they can be.

## 2. Principe of work

#### 2.1 Startup:

When starting up the protocol, it can consume a list of node definitions, each of them is a name associated with a socket address. The protocol will try establish a connection between each of the nodes. The logs are generated for further debug if anything goes wrong. After the startup process each instance of protocol (on each node) has its own set of nodes, most of them are remote and one is real. The real node is the current node running the protocol. The rest are the nodes the real one connects to.

#### 2.2 Working with protocol:

As already said, each of protocol instances has a list of nodes it connects to. The each node is accessible through the protocol instance, meaning that it is possible to execute an operation from one note onto the other one. For this purpose the user must get the node instance for the remote one and call proper function for operation execution. If the node is a remote physical machine, then the operation will be transferred to the other node and then executed on it.

If the other node uses a different version of operation set then it probably won't execute the operation. The reason might be that on one side we have an operation which doesn't exist on the other end.

There might be also connection issues, e.g. a node in the connected cluster becomes non-reachable.

For these kind of problems the protocol provides a basic set of operations, which are injected into the protocol (but also are kept as a separate module to provide maximal flexibility).

## 3. Contribution

The protocol is quite new and any kind of contribution is appreciated. You can contribute the project by opening issues or pull requests. Also giving any idea can lead to a massive improvement. Please try to contribute a clean code for others to be easy to read and understand. Though not clean code will be reviewed and fixed.

## 4. Roadmap

For the first iteration I want the protocol to support only few features that it promises, but do it very properly and without any issues. I think it will make possible to enlarge the bounds of the project very easily.

## 5. Third party libraries

The project currently is based on 3 libraries from **GitHub**. I'm planning to have this also a modular point, where the user can use the library they prefer.

- [sockpp](https://github.com/fpagliughi/sockpp/) from [fpagliughi](https://github.com/fpagliughi) for `socket` manipulation

- [json](https://github.com/nlohmann/json) from [nlohmann](https://github.com/nlohmann) for `json` manipulation
- [spdlog](https://github.com/gabime/spdlog) from [gabime](https://github.com/gabime) for logging

### 6. Licensing

```markdown
MIT License

Copyright (c) 2020 Arsen Gharagyozyan

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
