#! /usr/bin/env python
import sys
from sys import argv
import random

# Basado en codigo por Kelwin 

try:
	num_candidatos = int(argv[1])
	num_votos = int(argv[2])
	prof_nf = str(argv[3])
except:
 	print "./"+argv[0]+" num_candidatos num_votos profile.txt"
	sys.exit(1)


candidatos = []
for i in range(num_candidatos):
	c = chr(ord('a')+i)
	candidatos.append(c)

def get_perfil():
	perfil = []
	for x in range(num_votos):
		l = candidatos[:]
		random.shuffle(l)
		perfil.append(l)
	def cmp_it(x,y):
		for i in range(len(x)):
			if x[i] <> y[i]:
				return ord(x[i])-ord(y[i])
		return 0
	sperfil = sorted(perfil,cmp_it)
	p = sperfil[0]
	n = 1
	compact_perfil = []
	for pref in range(1,len(sperfil)):
		if sperfil[pref] == p:
			n += 1
		else:
			compact_perfil.append((n,p))
			p = sperfil[pref]
			n = 1
	compact_perfil.append((n,p))
	return compact_perfil


def calc_n_x_y(x,y,compact_p):
    res = 0
    for (n,pref) in compact_p:
        saw_x = -1
        saw_y = -1
        i = 0
        for z in pref:
            if z == x:
                 saw_x = i
            if z == y:
                 saw_y = i
            i += 1
        if saw_x < saw_y:
            res += n
    return res

def print_prof(compact_perfil,nf):
	f = file(nf,'w')
	print >> f, "profile"

	print >> f, num_candidatos,
	for c in candidatos:
		print >> f, c,
	print >> f

	print >> f, num_votos

	for (n,p) in compact_perfil:
		print >> f, n,
		for x in p:
			print >> f, x,
		print >> f


def print_nx(candidatos,compact_p):
	for x in candidatos:
	    for y in candidatos:
		if x <> y:
		    print 'N('+x+','+y+') =', calc_n_x_y(x,y,compact_p), ' ',
	    print


def get_condorcet(candidatos,num_votos,compact_p):
	limit = num_votos/2
	smallest_diff = num_votos
	got = False
	for x in candidatos:
		won = True
		for y in candidatos:
			if x <> y:
				nxy = calc_n_x_y(x,y,compact_p)
				if nxy <= limit:
					won = False
					break
				else:
					smallest_diff = min(smallest_diff,nxy)
		if won:
			return x,smallest_diff
	if not got:
		return None,None


def get_almost_condorcet(candidatos,num_votos,compact_p):
	limit = num_votos/2
	smallest_smallest_diff = -1
	got = False
	for x in candidatos:
		won = True
		smallest_diff = num_votos+1000
		for y in candidatos:
			if x <> y:
				nxy = calc_n_x_y(x,y,compact_p)
				if nxy <= limit:
					won = False
					#break
				smallest_diff = min(smallest_diff,nxy)
		if won:
			return True,x,smallest_diff
		if smallest_diff > smallest_smallest_diff:
			smallest_smallest_diff = smallest_diff
			best_so_far = x
	if not got:
		return False,best_so_far,smallest_smallest_diff

def get_no_condorcet(candidatos,num_votos,limit=0):
	global prof_nf
	got = False
	i = 1
	while not got:
		compact_perfil = get_perfil()
		is_cw,x,nxy = get_almost_condorcet(candidatos,num_votos,compact_perfil)
		if not is_cw:
			print_prof(compact_perfil,prof_nf)
			print_nx(candidatos,compact_perfil)
			print 'Profiled without condorcet winner saved in',prof_nf
			return x,nxy,compact_perfil
		else:
			print 'Had a condorcet winner. Candidate #'+str(i)
		i +=1

x,nxy,compact_perfil = get_no_condorcet(candidatos,num_votos)

