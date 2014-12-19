This is the 'receiver' for the unit tests.  Run it on a node which
is NOT under test.

TODO

Send finder request needs a re-think.  It needs to be re-implemented as a Tictocs::Timer, so it
doesn't block the loop().

I could also make Tictocs objects which monitor the RF24network and pull off messages that are just
for them.  Likewise, the network can raise a signal when there is a new message.  Although I would not
want to put that into the library for fear of adding a dependency.

so the finder needs to maintain state
- Waiting: Waiting for a finder request
- Sending: In the midst of looping through children

Needs to have some delay between each send.  So the finder has a timer component.

Also...  Reset should also reset the internal copy of the sync data.  Sync may need a 'reset' method.
