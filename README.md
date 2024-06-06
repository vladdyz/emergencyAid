# Emergency Aid Management System
An emergency aid management system that reads and writes to files, registering inventory and shipping order data.

Reads and writes pharmaceutical product records from/to multiple data files containing pertinent information regarding their usage, handling instructions, price, expiry dates, and availability. Discriminates between perishable and non-perishable items based on SKU, storing the data differently depending on the type. Default delimiting character is a tab (\t). Record format is SKU, item description, quantity available, quantity needed, price, handling instructions, and expiry dates (last two fields are only required for perishable items). 

Creates and exports shipping order information for products, keeping track of client needs and stock.

Contains functionality to add, remove, update, and sort through product data structures by navigating a simple menu system


