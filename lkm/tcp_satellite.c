#include <linux/mm.h>
#include <linux/module.h>
#include <net/tcp.h>

static void satellite_init(struct sock *sk)
{
	// tcp_sk(sk)->snd_ssthresh = 1000000;
}

static u32 satellite_ssthresh(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	return max(tp->snd_cwnd / 2, 2U);
}

void satellite_cong_avoid(struct sock *sk, u32 ack, u32 acked)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (!tcp_is_cwnd_limited(sk))
		return;

	/* In "safe" area, increase. */
	if (tcp_in_slow_start(tp)) {
		acked = tcp_slow_start(tp, acked);
		if (!acked)
			return;
	}
	/* In dangerous area, increase slowly. */
	tcp_cong_avoid_ai(tp, tp->snd_cwnd, acked);
}

static struct tcp_congestion_ops satellite __read_mostly = {
	.init = satellite_init,
	.ssthresh = satellite_ssthresh,
	.cong_avoid = satellite_cong_avoid,
	.undo_cwnd = tcp_reno_undo_cwnd,
	.owner = THIS_MODULE,
	.name = "satellite",
};

static int __init satellite_register(void)
{
	return tcp_register_congestion_control(&satellite);
}

static void __exit bictcp_unregister(void)
{
	tcp_unregister_congestion_control(&satellite);
}

module_init(satellite_register);
module_exit(bictcp_unregister);
MODULE_AUTHOR("gipsyh");
MODULE_LICENSE("GPL");