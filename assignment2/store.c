#include "backend.h"

void PrintLine(char* msg)
{
    printf("%s\n", msg);
}

IOOPM_CHOICE MainMenu()
{
    IOOPM_CHOICE choices[10] = { IOOPM_QUIT, IOOPM_ADD, IOOPM_LIST, IOOPM_REMOVE, IOOPM_EDIT, IOOPM_STOCK, IOOPM_REPLENISH, IOOPM_CREATE_CART, IOOPM_REMOVE_CART, IOOPM_EDIT_CART};
    
    PrintLine("-------------------------");
    PrintLine("(1) Add Merchandise");
    PrintLine("(2) List Merchandise");
    PrintLine("(3) Remove Merchandise");
    PrintLine("(4) Edit Merchandise");
    PrintLine("(5) Show Stock");
    PrintLine("(6) Replenish");
    PrintLine("(7) Create A Cart");
    PrintLine("(8) Remove A Cart");
    PrintLine("(9) Edit Cart");
    PrintLine("(0) Quit");
    PrintLine("-------------------------");
    
    int choice = 0;
    
    do
    {
        choice = ReadInt();
    } 
    while(choice < 0 || choice > 9);
    
    return choices[choice];
}

IOOPM_CHOICE CartMenu()
{
    PrintLine("-------------------------");
    PrintLine("(1) Add To Cart");
    PrintLine("(2) Remove From Cart");
    PrintLine("(3) Calculate Cost");
    PrintLine("(4) Checkout");
    PrintLine("(5) Return To Main Menu");
    PrintLine("-------------------------");
    
    return IOOPM_ADD;
}


void AddMerchandise(warehouse_t* wh)
{
    merch_t m;
    
    PrintLine("-------------------------");
    PrintLine("Enter the merchs name");
    char* name = ReadString();
    
    PrintLine("Enter the merchs description");
    char* description = ReadString();
    
    PrintLine("Enter the merchs price");
    int price = ReadInt();
    
    PrintLine("Enter location");
    char* location = ReadShelf();
    PrintLine("-------------------------");
    
    m.name = name;
    m.desc = description;
    m.price = price;
    m.location = location;
    m.amount = 0;
    
    ioopm_hash_table_insert(wh->items, StringValue(name), MerchValue(m));
}

void ListMerchadise(warehouse_t* wh)
{
    ioopm_list_t items = ioopm_hash_table_values(wh->items);
    
    size_t size = ioopm_hash_table_size(wh->items);
    
    merch_t arritems[size];
    
    for (size_t i = 0; i < size; i++)
    {
        elem_t tmp;
        ioopm_linked_list_get(items, i, &tmp);
        
        arritems[i] = tmp.merch;
    }
    
    Sort(arritems, size);
}

void EditMerchandise(warehouse_t* wh)
{
    
}

void RemoveMerchandise(warehouse_t* wh)
{
    
}

void ShowStock(warehouse_t* wh, merch_t* m)
{
    
}

void Replenish(warehouse_t* wh)
{
    
}

cart_t* CreateCart(warehouse_t* wh)
{
    return NULL;
}

void RemoveCart(cart_t* cart)
{
    
}

void AddToCart(cart_t* c, merch_t m)
{
    
}

void RemoveFromCart(cart_t* c, merch_t m)
{
    
}

int CalculateCost(cart_t* cart)
{
    return 420;
}

void Checkout(warehouse_t* wh, cart_t* cart)
{
    
}

int main()
{
    bool running = true;
    
    warehouse_t* wh = CreateWarehouse();
    
    while(running)
    {
        IOOPM_CHOICE c = MainMenu();
        
        switch(c)
        {
            case IOOPM_QUIT:
                running = false;
                break;
            case IOOPM_ADD:
                AddMerchandise(wh);
                break;
            case IOOPM_REMOVE:
                RemoveMerchandise(wh);
                break;
            
            default:
                return 0;
            
        }
    }
    
    DestroyWarehouse(wh);
}