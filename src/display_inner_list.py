def display_inner_list(inner_list, layer):
    for item in inner_list:
        if isinstance(item, list):
            display_inner_list(item, layer+1)
        else:
            print(item)
