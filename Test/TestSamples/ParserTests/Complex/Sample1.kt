/**
 * Comments
 */

val n = 5
val m = 10

fun sqrDist(x1 : Float, y1 : Float, x2 : Float, y2 : Float) : Float = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)

fun main() {
    println("Hello, world!!!")

    for (i in 0..n) {
        for (j in 0..m)
        	println(sqrDist(0.5f, 0.5f, 1f / n * i, 1f / m * j))
        println()
    }
}