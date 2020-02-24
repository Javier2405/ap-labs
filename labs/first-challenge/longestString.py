def longestString(cad):
	letras = []
	largest = 0
	for i in range(len(cad)):
		for j in range(i,len(cad)):
			if(cad[j] not in letras):
				letras.append(cad[j])
			else:
				if(largest < len(letras)):
					largest = len(letras)
				letras = []
				break
	print(largest)

#longestString("abcabcbb")
#longestString("bbbbb")
#longestString("pwwkew")

longestString("fealaquelolea")