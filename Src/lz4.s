/*
 * Honestly stolen from Jens Bauer, with some modifications.
 * https://community.arm.com/arm-community-blogs/b/architectures-and-processors-blog/posts/lz4-decompression-routine-for-cortex-m0-and-later
 */

                    .syntax             unified
                    .cpu                cortex-m4
                    .thumb
/* License: Public Domain - I cannot be held responsible for what it does or does not do if you use it, whether it's modified or not. */
/* Entry point = lz4_decompress. On entry: r0 = source, r1 = destination, r2 = length of decompressed data. */
                    .func               lz4_decompress
                    .global             lz4_decompress
                    .type               lz4_decompress,%function
                    .thumb_func
lz4_decompress:     push                {r4-r6,lr}          /* save r4, r5, r6 and return-address */
                    add                 r5,r2,r1            /* point r5 to end of decompressed data */
get_token:          ldrb                r6,[r0]             /* get token */
                    add                 r0,r0,#1            /* advance source pointer */
                    lsrs                r4,r6,#4            /* get literal length, keep token in r6 */
                    beq                 get_offset          /* jump forward if there are no literals */
                    bl                  get_length          /* get length of literals */
                    mov                 r2,r0               /* point r2 to literals */
                    bl                  copy_data           /* copy literals (r2=src, r1=dst, r4=len) */
                    mov                 r0,r2               /* update source pointer */
get_offset:         ldrb                r3,[r0,#0]          /* get match offset's low byte */
                    sub                 r2,r1,r3            /* subtract from destination; this will become the match position */
                    ldrb                r3,[r0,#1]          /* get match offset's high byte */
                    lsl                 r3,r3,#8            /* shift to high byte */
                    sub                 r2,r2,r3            /* subtract from match position */
                    add                 r0,r0,#2            /* advance source pointer */
                    and                 r4,r6,#0x0f         /* get rid of token's high 28 bits */
                    bl                  get_length          /* get length of match data */
                    add                 r4,r4,#4            /* minimum match length is 4 bytes */
                    bl                  copy_data           /* copy match data (r2=src, r1=dst, r4=len) */
                    cmp                 r1,r5               /* check if we've reached the end of the decompressed data */
                    blt                 get_token           /* if not, go get the next token */
                    pop                 {r4-r6,pc}          /* restore r4, r5 and r6, then return */
                    .thumb_func
get_length:         cmp                 r4,#0x0f            /* if length is 15, then more length info follows */
                    bne                 got_length          /* jump forward if we have the complete length */
get_length_loop:    ldrb                r3,[r0]             /* read another byte */
                    add                 r0,r0,#1            /* advance source pointer */
                    add                 r4,r4,r3            /* add byte to length */
                    cmp                 r3,#0xff            /* check if end reached */
                    beq                 get_length_loop     /* if not, go round loop */
got_length:         bx                  lr                  /* return */
                    .thumb_func
copy_data:          rsb                 r4,r4,#0            /* index = -length */
                    sub                 r2,r2,r4            /* point to end of source */
                    sub                 r1,r1,r4            /* point to end of destination */
copy_data_loop:     ldrb                r3,[r2,r4]          /* read byte from source_end[-index] */
                    strb                r3,[r1,r4]          /* store byte in destination_end[-index] */
                    adds                r4,r4,#1            /* increment index */
                    bne                 copy_data_loop      /* keep going until index wraps to 0 */
                    bx                  lr                  /* return */
                    .size               lz4_decompress,.-lz4_decompress
                    .endfunc
/* 39 instructions, 88 bytes */
