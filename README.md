# Inventory-Microservice
This microservice allows a user to maintain a database of items.

When inventory.exe is initialized, two text files inventory.txt and invcommand.txt will be added to your documents folder (Windows users).
You interact with invcommand.txt to interact with the service.

To add or remove an item, enter the name of the item, the math operator (+/-), and the amount as a string with no spaces. 
The service will add that item to your inventory.

To view your inventory, enter "read". 
Your inventory will be returned in invcommand.txt as a string that can be parsed by the key-value pairs and comma delimiter as you see fit.
