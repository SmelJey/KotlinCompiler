fun main() {
	val a:String = "${ "abc" }"
    val b:String = "${"${a}"}"
	val c:String = "${
    "${"$a"
    }"
    }"
	val d:String = "abcd ${ "abcd{}" } + ${ "test + ${ a + b + "${ "nested" }" }"} "


	val e:String = """
    ${ "abc" }
    """
	val f:String = "${ 
	"""abc
	dab""" 
	}"
    val g:String = """${"""${a}""""}""""
	val h:String = """${
    "${"$a"
    }"
    }"""
	val k:String = """abcd ${ "abcd{}" } + ${ "test + ${ a + b + "${ """
    nested
    test ${"another nest"}""" }" }"} """
    println(k)
}
