fun main() {
	val a = 1 + if (3 > 2) {
		println("first branch")
		4 + 5
	} else {
		println("second branch")
		4 - 5
	} 
	println(a)
}