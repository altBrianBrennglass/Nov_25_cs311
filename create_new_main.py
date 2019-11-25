main_ptr = open("main.c", "r")
new_file_ptr = open("full_main.c", "w")
scanning = True
for line in main_ptr:
	#if scanning:	
		test_line = line.split('"')
		try:
			isFore = test_line[1].split('/')
			if isFore[0] != "Fore" and isFore[0] != "home":
				header = open(isFore[0] + "/" + isFore[1], "r")
				new_file_ptr.write("\n\n")
				for line in header:
					new_file_ptr.write(line)
				header.close()
			else:
				raise
		except:
			new_file_ptr.write(line)



main_ptr.close()
new_file_ptr.close()
