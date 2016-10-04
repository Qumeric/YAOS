.text
.code64
.global interruption_handlers
.global handle_interruption

#define SAVE\
    pushq %rax;\
pushq %rbx;\
pushq %rcx;\
pushq %rdx;\
pushq %rbp;\
pushq %rdi;\
pushq %rsi;\
pushq %r9;\
pushq %r10;\
pushq %r11;\
pushq %r12;\
pushq %r13;\
pushq %r14;\
pushq %r15;

#define LOAD\
    popq %r15;\
popq %r14;\
popq %r13;\
popq %r12;\
popq %r11;\
popq %r10;\
popq %r9;\
popq %rsi;\
popq %rdi;\
popq %rbp;\
popq %rdx;\
popq %rcx;\
popq %rbx;\
popq %rax;

CALL_HANDLER:
    SAVE;
    movq %rsp, %rdi;
    cld;
    call handle_interruption;
    LOAD;
    add $16, %rsp;
    iretq


#define ExceptionHandler(id) \
    handler##id:      \
        pushq $(id);        \
        jmp CALL_HANDLER;

#define InterruptionHandler(id) \
    handler##id:      \
        pushq $(0);         \
        pushq $(id);        \
        jmp CALL_HANDLER;

InterruptionHandler(0)
InterruptionHandler(1)
InterruptionHandler(2)
InterruptionHandler(3)
InterruptionHandler(4)
InterruptionHandler(5)
InterruptionHandler(6)
InterruptionHandler(7)
ExceptionHandler(8)
InterruptionHandler(9)
ExceptionHandler(10)
ExceptionHandler(11)
ExceptionHandler(12)
ExceptionHandler(13)
ExceptionHandler(14)
InterruptionHandler(15)
InterruptionHandler(16)
ExceptionHandler(17)
InterruptionHandler(18)
InterruptionHandler(19)
InterruptionHandler(20)
InterruptionHandler(21)
InterruptionHandler(22)
InterruptionHandler(23)
InterruptionHandler(24)
InterruptionHandler(25)
InterruptionHandler(26)
InterruptionHandler(27)
InterruptionHandler(28)
InterruptionHandler(29)
InterruptionHandler(30)
InterruptionHandler(31)
InterruptionHandler(32)
InterruptionHandler(33)
InterruptionHandler(34)
InterruptionHandler(35)
InterruptionHandler(36)
InterruptionHandler(37)
InterruptionHandler(38)
InterruptionHandler(39)
InterruptionHandler(40)
InterruptionHandler(41)
InterruptionHandler(42)
InterruptionHandler(43)
InterruptionHandler(44)
InterruptionHandler(45)
InterruptionHandler(46)
InterruptionHandler(47)
InterruptionHandler(48)
InterruptionHandler(49)
InterruptionHandler(50)
InterruptionHandler(51)
InterruptionHandler(52)
InterruptionHandler(53)
InterruptionHandler(54)
InterruptionHandler(55)
InterruptionHandler(56)
InterruptionHandler(57)
InterruptionHandler(58)
InterruptionHandler(59)
InterruptionHandler(60)
InterruptionHandler(61)
InterruptionHandler(62)
InterruptionHandler(63)

interruption_handlers:
  .quad handler0
  .quad handler1
  .quad handler2
  .quad handler3
  .quad handler4
  .quad handler5
  .quad handler6
  .quad handler7
  .quad handler8
  .quad handler9
  .quad handler10
  .quad handler11
  .quad handler12
  .quad handler13
  .quad handler14
  .quad handler15
  .quad handler16
  .quad handler17
  .quad handler18
  .quad handler19
  .quad handler20
  .quad handler21
  .quad handler22
  .quad handler23
  .quad handler24
  .quad handler25
  .quad handler26
  .quad handler27
  .quad handler28
  .quad handler29
  .quad handler30
  .quad handler31
  .quad handler32
  .quad handler33
  .quad handler34
  .quad handler35
  .quad handler36
  .quad handler37
  .quad handler38
  .quad handler39
  .quad handler40
  .quad handler41
  .quad handler42
  .quad handler43
  .quad handler44
  .quad handler45
  .quad handler46
  .quad handler47
  .quad handler48
  .quad handler49
  .quad handler50
  .quad handler51
  .quad handler52
  .quad handler53
  .quad handler54
  .quad handler55
  .quad handler56
  .quad handler57
  .quad handler58
  .quad handler59
  .quad handler60
  .quad handler61
  .quad handler62
  .quad handler63
