#include "backend.h"

struct list_entry
{
    elem_t val;
    list_entry_t* next;
    list_entry_t* prev;
};

struct list
{
    size_t size;
    list_entry_t* sentinel;
    list_entry_t* last;

    ioopm_eq_function eqfunc;
};

IOOPM_CHOICE main_menu()
{
    IOOPM_CHOICE choices[10] = { IOOPM_QUIT, IOOPM_ADD, IOOPM_LIST, IOOPM_REMOVE, IOOPM_EDIT, IOOPM_STOCK, IOOPM_REPLENISH, IOOPM_CREATE_CART, IOOPM_REMOVE_CART, IOOPM_EDIT_CART};
    
    print_line("-------------------------");
    print_line("(1) Add Merchandise");
    print_line("(2) List Merchandise");
    print_line("(3) Remove Merchandise");
    print_line("(4) Edit Merchandise");
    print_line("(5) Show Stock");
    print_line("(6) replenish");
    print_line("(7) Create A Cart");
    print_line("(8) Remove A Cart");
    print_line("(9) Edit Cart");
    print_line("(0) Quit");
    print_line("-------------------------");
    
    int choice = read_int_range(0, 9);
    
    return choices[choice];
}

IOOPM_CHOICE cart_menu()
{
    print_line("-------------------------");
    print_line("(1) Add To Cart");
    print_line("(2) Remove From Cart");
    print_line("(3) Calculate Cost");
    print_line("(4) Checkout");
    print_line("(5) Return To Main Menu");
    print_line("-------------------------");
    
    return IOOPM_ADD;
}


void add_merchandise(warehouse_t* wh)
{
    merch_t m;
    
    print_line("-------------------------");
    print_line("Enter the merchs name");
    char* name = read_string();
    
    print_line("Enter the merchs description");
    char* description = read_string();
    
    print_line("Enter the merchs price");
    int price = read_int();
    
    print_line("Enter location");
    char* location = read_shelf();
    print_line("-------------------------");
    
    m.name = name;
    m.desc = description;
    m.price = price;
    m.location = location;
    m.amount = 0;
    
    ioopm_hash_table_insert(wh->items, string_value(location), merch_value(m));
}

void list_merchandise(warehouse_t* wh)
{
    ioopm_list_t* items = ioopm_hash_table_values(wh->items);
    
    size_t size = ioopm_hash_table_size(wh->items);
    
    if(size == 0)
        print_line("You have no merch");
    
    merch_t arritems[size];
    
    for (size_t i = 0; i < size; i++)
    {
        elem_t tmp;
        ioopm_linked_list_get(items, i, &tmp);
        
        arritems[i] = tmp.merch;
    }
    
    sort(arritems, size);
    
    int counter = 0;
    
    for (int i = 0; i < size; i += 20) 
    {
        for (int j = 0; j < 20; j++) 
        {
            if(i + j < size)
            {
                print_merch(arritems[i + j]);
                printf("\n");
            }
                
            counter++;
        }
        
        if(counter >= size)
        {
            break;
        }
        
        print_line("Continue showing entries? (y/n)");
        bool showmore = read_bool();
        
        if(!showmore)
            break;    
    }
    
    print_line("Press enter to continue");
    getchar();
    
    ioopm_linked_list_destroy(items);
}

void edit_merchandise(warehouse_t* wh)
{
    char* choice = ask_user_for_name(wh);
    
    elem_t old;
    ioopm_hash_table_lookup(wh->items, string_value(choice), &old);
    
    print_line("-------------------------");
    print_merch(old.merch);
    print_line("-------------------------");
    
    merch_t m;
    
    print_line("-------------------------");
    print_line("Enter the new name");
    char* name = read_string();
    
    print_line("Enter the new description");
    char* description = read_string();
    
    print_line("Enter the new price");
    int price = read_int();
    
    print_line("Enter the new location");
    char* location = read_shelf();
    print_line("-------------------------");
    
    m.name = name;
    m.desc = description;
    m.price = price;
    m.location = location;
    m.amount = 0;
    
    ioopm_hash_table_remove(wh->items, string_value(choice), NULL);
    ioopm_hash_table_insert(wh->items, string_value(name), merch_value(m));
}

void remove_merchandise(warehouse_t* wh)
{
    char* name = ask_user_for_name(wh);
    
    ioopm_hash_table_remove(wh->items, string_value(name), NULL);
}

void show_stock(warehouse_t* wh)
{
    print_line("Enter merch name");
    char* name = read_string();
    
    merch_t m;
    m.name = name;
    m.location = "Kappa";
    m.amount = 0;
    m.price = 420;
    m.desc = "fake item";
    
    ioopm_list_t* items = ioopm_hash_table_values(wh->items);
    ioopm_list_t* correct_items = ioopm_linked_list_create(merch_compare);
    
    size_t size = ioopm_hash_table_size(wh->items);
    
    for (size_t i = 0; i < size; i++) 
    {
        elem_t tmp;
        
        ioopm_linked_list_get(items, i, &tmp);
        
        if(merch_compare(tmp, merch_value(m)))
        {
            ioopm_linked_list_append(correct_items, tmp);
        }
    }
    
    ioopm_linked_list_destroy(items);
    
    size_t sort_size = ioopm_linked_list_size(correct_items);
    
    if(sort_size == 0)
    {
        print_line("You have no merchandise of that name!");
        print_line("Press enter to continue");
        getchar();
        
        return;
    }    
    merch_t merches[size];
    
    for (size_t i = 0; i < sort_size; i++) 
    {
        elem_t val;
        ioopm_linked_list_get(correct_items, i, &val);
        
        merches[i] = val.merch;
    }
    
    sort(merches, sort_size);
    
    printf("Listing for %s\n", m.name);
    
    for (size_t i = 0; i < sort_size; i++) 
    {
        merch_t m = merches[i];
        
        printf("%s:%zu\n", m.location, m.amount);
    }
    
    print_line("Press enter to continue");
    getchar();
}

void replenish(warehouse_t* wh)
{
    print_line("Where do you want to replenish?");
    char* location = read_shelf();
    
    bool exists = ioopm_hash_table_has_key(wh->items, string_value(location));
    
    if(exists)
    {
        print_line("What should the new amount be?");
        int new_amount = read_int();
        
        elem_t val;
        ioopm_hash_table_lookup(wh->items, string_value(location), &val);
        
        val.merch.amount = new_amount;
        
        ioopm_hash_table_insert(wh->items, string_value(location), val);
    }
    else
    {
        print_line("Merch not found. Do you want to add something there? (y/n)");
        
        if(read_bool())
        {
            merch_t m; 
    
            print_line("Enter the merchs name");
            char* name = read_string();
    
            print_line("Enter the merchs description");
            char* description = read_string();
    
            print_line("Enter the merchs price");
            int price = read_int();
            
            print_line("What should the amount be?");
            int amount = read_int();
            
            m.name = name;
            m.desc = description;
            m.price = price;
            m.location = location;
            m.amount = amount;
    
            ioopm_hash_table_insert(wh->items, string_value(location), merch_value(m));
        }
        else
        {
            return;
        }
    }
    
}

cart_t* create_cart(warehouse_t* wh)
{
    return NULL;
}

void remove_cart(cart_t* cart)
{
    
}

void add_to_cart(cart_t* c, merch_t m)
{
    
}

void remove_from_cart(cart_t* c, merch_t m)
{
    
}

int calulate_cost(cart_t* cart)
{
    return 420;
}

void checkout(warehouse_t* wh, cart_t* cart)
{
    
}

int main()
{
    bool running = true;
    
    warehouse_t* wh = create_warehouse();
    
    while(running)
    {
        IOOPM_CHOICE c = main_menu();
        
        switch(c)
        {
            case IOOPM_QUIT:
                running = false;
                break;
            case IOOPM_ADD:
                add_merchandise(wh);
                break;
            case IOOPM_REMOVE:
                remove_merchandise(wh);
                break;
            case IOOPM_LIST:
                list_merchandise(wh);
                break;
            case IOOPM_EDIT:
                edit_merchandise(wh);
                break;
            case IOOPM_STOCK:
                show_stock(wh);
                break;
            case IOOPM_REPLENISH:
                replenish(wh);
                break;
            
            default:
                running = false;
                break;
            
        }
    }
    
    destroy_warehouse(wh);
}