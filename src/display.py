def display(inner_list, layer):
    print "layer: ", layer
    for item in inner_list:
        if isinstance(item, list):
            display(item, layer+1)
        else:
            print item
