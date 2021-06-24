fun test1() = 5
fun test2() : Int = 3
fun test3() {
	return 5
}
fun test4() : Int {
	return 3;
}

fun main() {
	val a = test1()
	val b = test2()
	val c = test3()
}