class A {
	var myA = 10

	fun test() : Int = myA
}
class B {
	var a = A()

	fun testB() : A = a
    fun testA() : Int = a.myA
}


fun main() {	
	val b = B()
	b.a.myA = 5
	println(B().testB().test())
    println(b.testB().test())
    println(b.testA())
}
