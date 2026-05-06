# CONSTANTES POUR :

# Déplacement (mesurées à la main)
CM_PAR_SEC = 81
DEG_PAR_SEC = 90  # À mesurer

# Seuils de sécurité (cm)
SEUIL_AVANT = 20
SEUIL_LATERAL = 20
DIST_MUR = 25
TOLERANCE_MUR = 5

def duree_pour_distance(cm):
    return cm / CM_PAR_SEC

def duree_pour_angle(deg):
    return deg / DEG_PAR_SEC