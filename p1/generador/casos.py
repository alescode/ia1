from sys import argv
import random

try:
	num_candidatos = int(argv[1])
	num_preferencias = int(argv[2])
	num_electores = int(argv[3])

	print "profile"

	print num_candidatos,

	candidatos = []
	for i in range(num_candidatos):
		candidatos += ["a"*(i+1)]
		print "a"*(i+1),

	print

	print num_preferencias

	while (num_preferencias > 0):
		num = random.randint(1, num_electores)

		print num,

		rango = len(candidatos)
		for r in range(len(candidatos)):
			cual = int(random.uniform(0,len(candidatos)-r))
			print candidatos[cual],
			
			candidatos[len(candidatos) -1 - r], candidatos[cual] = candidatos[cual], candidatos[len(candidatos) -1 - r]
		print

		num_preferencias -= 1

except:
	print "python casos.py num_candidatos num_preferencias max_electores_por_preferencia"
