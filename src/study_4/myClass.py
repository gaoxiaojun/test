class FooClass(object):
    """My very first class: FooClass"""
    version = 0.1 # class (data) attribute
    def __init__(self, nm='John Doe'):
	"""Constructor"""
	self.name = nm # class instance (data) attribute
	print 'Created a class instance for', nm
    def showname(self):
	"""Display instance attribute and class name"""
	print 'Your name is', self.name
	print 'My name is', self.__class__.__name__
    def showver(self):
	"""Display class (static) aattribute"""
	print self.version # reference FooClass.version
    def addMe2Me(self, x): # does not use 'self'
	"""Apply + operation to argument"""
	return x + x
