/**
 * Comments
 */

val n = 5
val m = 10

class Point {
    var x : Double = 0.0
    var y : Double = 0.0
    
}

fun sqr(x : Double) : Double {
    return x * x
}
fun sqrDist(p1: Point, p2: Point) : Double = sqr(p2.x - p1.x) + sqr(p2.y - p1.y)

fun main() {
    println("Hello, world!!!")

    val p2 = Point()
    p2.x = 0.5
    p2.y = 0.5
    
    for (i in 0..n) {
        for (j in 0..m) {
            val p1 = Point()
            p1.x = 1.0 / n * i
            p2.y = 1.0 / m * j
            println(sqrDist(p1, p2))
        }
        	
        println()
    }
}