class A {
	var a = 2
	
	fun test() {
		println(++a)
	}
}

fun main() {
	val a = A()
	a.test()
	
	class A {
		var b = 10
		
		fun test() {
			println(++b)
		}
		
		fun test2() {
			println("test")
		}
	}
	
	val b = A()
	b.test()
	b.test2()
	a.test()
}
