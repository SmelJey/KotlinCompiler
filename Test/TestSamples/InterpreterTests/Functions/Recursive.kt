fun fact(n : Int) : Int {
	if (n == 1) {
		return n
	}
	
	return n * fact(n - 1)
}

fun main() {
	println(fact(5))
	println(fact(6))
}