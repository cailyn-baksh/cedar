#!/usr/bin/awk -f

BEGIN {
	isExternSymbol = 0
	externSymbolName = ""
}

/^\s*extern / {
	isExternSymbol = 1
}

{
	if (!isExternSymbol) {
		print
	} else {
		if (!($2 in externSymbols)) {
			# new extern
			externSymbols[$2] = NR
			print
		}
	}
}