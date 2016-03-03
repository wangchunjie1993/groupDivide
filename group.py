def main():
	institutes = {}
	f = open("students.csv",'r')
	f.readline()
	for line in f:
		instituteName = line.split(',')[4]
		if institutes.has_key(instituteName):
			institutes[instituteName].append(line)
		else:
			institutes[instituteName] = []
			institutes[instituteName].append(line)
	f.close()

	groups=[]
	groupNum = 5
	for i in range(groupNum):
		groups.append([])

	index = 0
	for institute in institutes:
		for record in institutes[institute]:
				groups[index].append(record)
				index = (index+1)%groupNum
	
	
	cnt = 1
	for group in groups:
		print '*'*50
		print "Group %d" % cnt

		cnt+=1
		for record in group:
			print record,
		print 

if __name__ == '__main__':
	main()