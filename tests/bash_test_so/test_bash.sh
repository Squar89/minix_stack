cd /usr/src/minix/drivers/examples/hello_stack && \
make && \
make install  && \
service up /service/hello_stack -dev /dev/hello_stack && \
 && \
cd ~ && \
make && \
 && \
echo && \
echo "Clearing device..." && \
cat /dev/hello_stack > /dev/null && \
echo "Test #1: urandom fill and empty" && \
dd if=/dev/urandom of=/dev/hello_stack bs=1000 count=1000 && cat /dev/hello_stack | wc -c | diff ./bash_test1.out - && \
echo "Test #2: zero fill and empty" && \
dd if=/dev/zero of=/dev/hello_stack bs=1000 count=1000 && cat /dev/hello_stack | wc -c | diff ./bash_test2.out - && \
echo "Test #3: self copy" && \
( echo 123 > /dev/hello_stack && dd if=/dev/hello_stack of=/dev/hello_stack && cat /dev/hello_stack ) |  diff ./bash_test3.out - && \
echo "Test #4: dd with skip" && \
dd if=/dev/hello_stack bs=1000 count=1 skip=1000 2>&1 | diff ./bash_test4.out - && \
echo "Test #5: dd with seek" && \
dd if=/dev/urandom bs=4096 seek=7 count=2 of=/dev/hello_stack | cat /dev/hello_stack | wc -c | diff ./bash_test5.out - && \
echo "Testing finished" && \
service down hello_stack

