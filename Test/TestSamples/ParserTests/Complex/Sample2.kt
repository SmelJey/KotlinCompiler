/**
 * Comments
 */

val n = 5
val m = 10

class Point {
    var x : Float = 0f
    var y : Float = 0f
    
}

fun sqr(x : Float) : Float {
    return x * x
}
fun sqrDist(p1: Point, p2: Point) : Float = sqr(p2.x - p1.x) + sqr(p2.y - p1.y)

fun main() {
    println("Hello, world!!!")

    val p2 = Point()
    p2.x = 0.5f
    p2.y = 0.5f
    
    for (i in 0..n) {
        for (j in 0..m) {
            val p1 = Point()
            p1.x = 1f / n * i
            p2.y = 1f / m * j
            println(sqrDist(p1, p2))
        }
        	
        println()
    }
}