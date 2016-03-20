#define ICMP6_DST_UNREACH                 1	/**< dest unreachable */
#define ICMP6_PACKET_TOO_BIG	            2	/**< packet too big */
#define ICMP6_TIME_EXCEEDED	            3	/**< time exceeded */
#define ICMP6_PARAM_PROB	               4	/**< ip6 header bad */
#define ICMP6_ECHO_REQUEST              128  /**< Echo request */
#define ICMP6_ECHO_REPLY                129  /**< Echo reply */

#define ICMP6_RS                        133  /**< Router Solicitation */
#define ICMP6_RA                        134  /**< Router Advertisement */
#define ICMP6_NS                        135  /**< Neighbor Solicitation */
#define ICMP6_NA                        136  /**< Neighbor advertisement */
#define ICMP6_REDIRECT                  137  /**< Redirect */

#define ICMP6_RPL                       155  /**< RPL */
#define ICMP6_PRIV_EXP_100              100  /**< Private Experimentation */
#define ICMP6_PRIV_EXP_101              101  /**< Private Experimentation */
#define ICMP6_PRIV_EXP_200              200  /**< Private Experimentation */
#define ICMP6_PRIV_EXP_201              201  /**< Private Experimentation */
#define ICMP6_ROLL_TM    ICMP6_PRIV_EXP_200  /**< ROLL Trickle Multicast */
