package motopgi.utils;



public final class ExceptionUtils {

	private ExceptionUtils() {
	}
	
	/**
	 * まとめてcloseする
	 * @param array
	 * @throws Exception 各closeで発生した例外をまとめたもの
	 */
	public static void close(AutoCloseable... array) throws Exception {
		// それぞれcloseして例外をまとめて投げる
		var exception = new Exception("closeに失敗しました");
		for(var e: array) {
			try {
				e.close();
			} catch (Exception ex) {
				exception.addSuppressed(ex);
			}
		}
		if (exception.getSuppressed().length > 0) {
			throw exception;
		}
	}

}
