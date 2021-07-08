/**
 * Comments
 */

val n = 5
val m = 10

fun sqrDist(x1 : Double, y1 : Double, x2 : Double, y2 : Double) : Double = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)

fun round(x : Double) : Int = (x * 10000.0).toInt()

fun main() {
    println("Hello, world!!!")

    for (i in 0..n) {
        for (j in 0..m)
        	println(round(sqrDist(0.5, 0.5, 1.0 / n * i, 1.0 / m * j)))
		println("row")
    }
}