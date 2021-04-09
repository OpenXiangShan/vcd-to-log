#set -x
echo Generating Xiang-Parser Sources ...
cd grammar && java -Xmx500M -cp "/usr/local/lib/antlr-4.9-complete.jar:$CLASSPATH" \
	org.antlr.v4.Tool -visitor -no-listener -Dlanguage=Cpp Xiang.g4 \
	-o ../build/xiang-parser
echo Copying new files to xiang-parser ...
cp ../build/xiang-parser/*.cpp ../xiang-parser/
cp ../build/xiang-parser/*.h ../xiang-parser/