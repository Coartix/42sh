# EpiTests is a Test Generator yaml code, made with love by Thomas Peugnet.

import os
import sys

index = input("Démarrer les tests à l'index : ")
stop = input("S'arrêter à l'index : ")
stop = int(stop)
index = int(index)

while (index < stop):
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

	file = open("tests/tests_suite/files_sh/file_" + str(index), "w")
	file.write(shell_code)
	file.close()

	file = open("tests/tests_suite/test.yml", "a")
	test_code = '\n---\nname : Test - ' + test_title + ' - FILE_' + str(index) + '\ncmd: bash -c "diff <(builddir/42sh tests/tests_suite/files_sh/file_' + str(index) + '.sh) <(sh tests/tests_suite/files_sh/file_' + str(index) + '.sh)"'
	file.write(test_code)
	file.close()

	print("Vérification de la validité du test numéro " + str(index) + " en cours...\n")
	os.system('make pharaoh')
	print(" -- Vérification terminée -- ")
	index = int(index) + 1
