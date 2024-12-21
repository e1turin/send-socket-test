
- https://www.ibm.com/docs/en/i/7.3?topic=sscaaiic-example-accepting-connections-from-both-ipv6-ipv4-clients
- https://www.ibm.com/docs/en/i/7.3?topic=clients-example-ipv4-ipv6-client


- > Что произойдет если вызвать send по сокету для которого уже вернули 0
- > Нужен эксперимент. Напишите короткую программу в которой продемонстрируйте что произойдет
  - > Как понять, при отправке send, что соединие закрыто?

- https://elixir.bootlin.com/linux/v4.7/source/net/socket.c#L1612


- stackoverflow
    - https://stackoverflow.com/a/32926280 & https://stackoverflow.com/a/37034095
    - man + posix https://stackoverflow.com/a/41970485
- man https://man7.org/linux/man-pages/man2/sendmsg.2.html
    - > No indication of failure to deliver is implicit in a send().
      > Locally detected errors are indicated by a return value of -1
- libc https://sourceware.org/glibc/manual/latest/html_mono/libc.html#Sockets
- posix https://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html


- https://stackoverflow.com/a/17705579
- https://stackoverflow.com/a/76394138/19036461


Никак гарантированно не узнать при send, что соединение закрыто. Нужно сделать
recv, который вернет успешный результат считывания 0.

