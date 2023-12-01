#!/bin/python3

import os
import sys

index = input("Démarrer les tests à l'index : ")
stop = input("S'arrêter à l'index : ")
stop = int(stop)
index = int(index)


while (index <= stop):
	print("Test numéro : " + str(index) + "\n")
	test_title = input("Entrer le titre du test : ")

	stout = input("Entrer stdout attendu : ")
	sterr = input("Entrer stderr attendu : ")
	status = input("Entrer status attendu : ")

	print("Entrer le code shell à tester :")
	shell_code = ""

	while(True):
		line = sys.stdin.readline()
		if line:
			shell_code += line
		else:
			break

	file = open("tests/tests_suite/argument_test/test" + str(index), "w")
	variable = 'comd="' + shell_code + '"'
	file.write(variable)
	file.close()


	file = open("tests/tests_suite/argument_test.yml", "a")

	test_code = '\n---\nname : Test' + str(index) + ' - ' + test_title + '\ncmd : . tests/tests_suite/argument_test/test' + str(index) + '; ./builddir/42sh -c \"$comd\"\nstdout : \"' + stout + '\\n\"\nstderr : \"' + sterr + '\\n\"\nstatus : ' + status + '\n'

	file.write(test_code)
	file.close()

	print("\nDone\n")
	index += 1
