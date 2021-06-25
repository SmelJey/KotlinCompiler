fun main() {
	1 in arrayOf<Int>(1, 2, 3)
	2.4 !in arrayOf<Double>(1.2, 2.4)
	1.5 in arrayOf<Int>(1, 2, 3)
	"a" !in arrayOf<Int>(1, 2, 3)
}