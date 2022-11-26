import sys
import os
import re
import subprocess

def main():
	Result = subprocess.run(["cloc", "../BigLib/SOURCE"], stdout=subprocess.PIPE, text=True)
	Lines = Result.stdout.split("\n")
	SeparatorLine = 0
	Matrix = []
	for Line in Lines:
		print(Line)
		Span = re.search("[^-]{0,}", Line).span()
		if Span[1] - Span[0] > 0:
			if SeparatorLine > 1:
				Vector = []
				for Number in re.split("[^(\\d{0,})]", Line):
					if len(Number) > 0:
						Vector.append(int(Number))
				if len(Vector) > 0:
					Matrix.append(Vector)
		else:
			SeparatorLine += 1
	Last = Matrix[-1]
	Dictionary = {
		"FILES_AMOUNT": Last[0],
		"LINES_BLANK": Last[1],
		"LINES_COMMENTS": Last[2],
		"LINES_COUNT": Last[3],
		"LINES_TOTAL": Last[1] + Last[2] + Last[3],
	}

	Template = ""
	with open("../README_TEMPLATE.md", "r") as TemplateFile:
		Template = TemplateFile.read()
		TemplateFile.close()

	for Key in Dictionary:
		Flag = "%" + Key + "%"
		print(Flag, "->", Dictionary[Key])
		Template = Template.replace(Flag, str(Dictionary[Key]))

	if os.path.isfile("../README.md"):
		os.remove("../README.md")
	with open("../README.md", "w") as Output:
		Output.write(Template)
		Output.close()

if __name__ == '__main__':
	main()