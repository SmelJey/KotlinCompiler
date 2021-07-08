fun test() {
	println("no arg")
}

fun test(a : Int) {
	println("int arg")
	println(a)
}

fun test(a : String) {
	println("string arg")
	println(a)
}

fun main() {
	val a = 2
	val b = "aba"
	
	test()
	test(a)
	test(b)
	println("end")
}