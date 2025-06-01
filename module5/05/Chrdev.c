#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>
#include <linux/string.h>

#define NETLINK_USER 31
#define MSG_LEN 1034

static int __init kern_init(void);
static void __exit kern_exit(void);
static void rcv_msg(struct sk_buff* sbuff);

struct sock *net_sock = NULL;

struct netlink_kernel_cfg conf = {
    .groups  = 1,
    .input = rcv_msg,
};

static void rcv_msg(struct sk_buff* sbuff) {
    struct nlmsghdr* net_header;
    int pid;
    struct sk_buff* soutbuff;
    int size;
    char* msg;
    int result;

    net_header = (struct nlmsghdr*) sbuff->data;
    printk(KERN_INFO "Received msg: %s\n", (char*) nlmsg_data(net_header));
    pid = net_header->nlmsg_pid;
    msg = kmalloc(MSG_LEN * sizeof(char), GFP_KERNEL);
    msg = strcat(msg, "Your msg:");
    msg = strcat(msg, nlmsg_data(net_header));
    
    size = strlen(msg);

    soutbuff = nlmsg_new(size, 0);
    if(soutbuff < 0) {
        printk(KERN_INFO "Error allocating response\n");
        kfree(msg);
        return;
    }
    net_header = nlmsg_put(soutbuff, 0, 0, NLMSG_DONE, size, 0);
    NETLINK_CB(soutbuff).dst_group = 0;
    strncpy(nlmsg_data(net_header), msg, size);

    result = nlmsg_unicast(net_sock, soutbuff, pid);
    if(result < 0) printk(KERN_INFO "Error sending to user\n");
    kfree(msg);
}
 
static int __init kern_init(void) {
   printk("Staring kernel module\n");
   net_sock = netlink_kernel_create(&init_net, NETLINK_USER, &conf);
   if(!net_sock) {
       printk(KERN_INFO "Error creating socket\n");
       return -10;
   }
   return 0;
}
static void __exit kern_exit(void) {
   printk(KERN_INFO "Exiting kernel module\n");
   netlink_kernel_release(net_sock);
}
module_init(kern_init);
module_exit(kern_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oblaukhov P. K.");