fun main() {
	var x = 0
	var y = 0
	do {
		var res = ""
		do {
			res += "o"
		} while (x++ < 5 && x <= y) 
		
		println(res)
        y++
        x = 0
	} while (y < 5)
}