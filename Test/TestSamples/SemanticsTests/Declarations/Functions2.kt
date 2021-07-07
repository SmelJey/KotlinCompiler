fun test4() : Int {
	return 3;
}

fun main() {
	fun test1() : Int = 5
	fun test3() : Int {
		fun test2() : Int = 3
		
		val a = test1()
		val b = test4()
		
		return 5
	}
	
	val a = test1()
	val b = test4()
	val c = test3()
}