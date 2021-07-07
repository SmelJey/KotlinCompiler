fun main() {
	var x = 0
	var y = 0
	while (y < 5) {
		var res = ""
		while (x++ < 5 && x <= y) {
			res += "o"
		}
		
		println(res)
        y++
        x = 0
	}
}