val a = 5

fun test(x : Int) : Int = x * a

fun main() {
	val arr = arrayOf<Int>(1, 2, 3)
	
	val a = 2
	
	println(test(arr[0]))
	println(test(arr[1]))
	println(test(arr[2]))
}