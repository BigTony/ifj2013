import sys
import re

if __name__ == "__main__":
	ok_alloc = 0
	ok_errors = 0
	for x in sys.stdin:
		if "total heap usage:" in x:
			y = x.split(" ")
			alloc = y[6]
			dealloc = y[8]
			if alloc == dealloc:
				ok_alloc = 1
		if "ERROR SUMMARY:" in x:
			y = x.split(" ")
			errors = y[3]
			e_from = y[6]
			if errors == e_from:
				ok_errors = 1
	
	result = "Allocated: " + alloc + "\t" + "Deallocated: " + dealloc + "\n" + "ERRORS: " + errors + "\t" + "From: " + e_from

	print ok_alloc
	print ok_errors
	print result



