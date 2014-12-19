These unit tests only work on Maple.  This test runs on a Maple,
as node #00.  One other node is expected on node #01, running the
"unit_rx" sketch, which runs on Arduino.

In the future, it would be interesting to write a test which enumerated
the entire tree.  Each node could implement a "child finder", where it
sends a message to each child.  Upon receiving the message, each child
sends the message out to ITS children, AND sends a "Hello" to the base node.

This requires a 'F' finder request.  For the response, we can re-use
the 'E' echo response.  The 'E' echo system presumes that the base is
the requestor and the RX nodes are the responder.
