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

	print("Entrer le code shell à tester :")
	shell_code = ""

	while(True):
		line = sys.stdin.readline()
		if line:
			shell_code += line
		else:
			break

	file = open("tests/tests_suite/argument_test/test" + str(index), "w")
	file.write(shell_code)
	file.close()


	file = open("tests/tests_suite/argument_test.yml", "a")

	test_code = '\n---\nname : Test' + str(index) + ' - ' + test_title + '\ncmd : bash -c "diff <( ./builddir/42sh < tests/tests_suite/argument_test/test' + str(index) + ') <(sh < tests/tests_suite/argument_test/test' + str(index) + ')"'

	file.write(test_code)
	file.close()

	print("\nDone\n")
	index += 1
