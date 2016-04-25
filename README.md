# GraphLang

A simple language and engine for expressing relationships between nodes and facts about those nodes.

Still in really early alpha.

## Usage
To add a new node the the graph, just declare it.
    
    (hunter);

To attach facts (attributes) to a node inlcude them in a declaration. Facts will also be attached to a preexisting node with the same syntax:

     (hunter name:"Hunter Fernandes" github:"hfern" favNumber:42);

Supported attribute data types are currently just strings and doubles. Integral types might be added later.

Relationships between nodes is declared in a similar way -- by "drawing" an ascii arrow with the name of the relationship between nodes. For example, to say the node "hunter" has a bidirectional relation called "friends" with a node named "kevin": 

    (hunter) <-friends> (kevin);

We can also declare new nodes and attributes in relation statements:

    (hunter) <-friends> (kevin name:"Kevin Bacon");

This will create a node identified by "kevin" and attach the name attribute and value if one does not already exist.

Group declaration and relation statements are also supported:
   
    [(hunter), (cruise name:"Tom Cruise"), (colbert name:"Stephen Colbert")];

    [(hunter), (cruise), (colbert)] <-friends-> [(kevin), (oprah name:"Oprah Winfrey")];

