# Networking Notes

## TCP byte stream and protocols

Network protocol is how information is relayed through network

TCP Protocal
    - Produces continuous stream of bytes
    - No internal boundaries
        - Intepreting **byte stream** is job of application protocol

**application protocol**
    - rules for interpreting byte stream, which includes how to split it into messages

## Data serialization

Message is something high level, like objects such as strings, structs, lists
... but computer networks only know 0 and 1s

Solution: Create a mapping between objects and bytes
    - This is called serialization (object to bytes) and deserialization (bytes to object)

# Concurrent Programming

Difficult, and has been difficult (C10K problem).
    - Haven't heard of it, but can imagine 10k concurrent connections being very difficult to handle, especially thorugh a thread or process based solution which I saw in ICS 53

Solution to this: Event based concurrency with event loops.
    - Also saw this in ICS 53. Something with multiplexor whatever. Harder to understand what happened behind the scenes but I'll trust the process for now
    - Pretty sure Node.JS runtime env uses it. Drives NGINX, Redis, and Golang's runtime as well.


