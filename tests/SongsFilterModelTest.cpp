#include <QTest>

#include "SongsModel.h"

namespace
{
    const auto SONG1_NAME = QStringLiteral("Hidden: low, today");
    const auto SONG2_NAME = QStringLiteral("1. low, yesterday");
    const auto SONG3_NAME = QStringLiteral("2. low, yesterday");
    const auto SONG4_NAME = QStringLiteral("Hidden: med, today");
    const auto SONG5_NAME = QStringLiteral("Hidden: med, yesterday");
    const auto SONG6_NAME = QStringLiteral("3. med, four days ago");
    const auto SONG7_NAME = QStringLiteral("Hidden: high, today");
    const auto SONG8_NAME = QStringLiteral("5. high, one week ago");
    const auto SONG9_NAME = QStringLiteral("4. high, 30 days ago");
}

class SongsFilterModelTest : public QObject
{
    Q_OBJECT

private slots:
    void init()
    {
        m_model = new SongsModel{this};
        m_model->setInstrument("Piano");

        auto s1 = new Song{this};
        s1->setName(SONG1_NAME);
        s1->setLastPracticed(QDateTime::currentDateTime());
        s1->setProficiency(Song::LowProficiency);
        m_model->addSong(s1);

        auto s2 = new Song{this};
        s2->setName(SONG2_NAME);
        s2->setLastPracticed(QDateTime::currentDateTime().addDays(-1));
        s2->setProficiency(Song::LowProficiency);
        m_model->addSong(s2);

        auto s3 = new Song{this};
        s3->setName(SONG3_NAME);
        s3->setLastPracticed(QDateTime::currentDateTime().addDays(-1));
        s3->setProficiency(Song::LowProficiency);
        m_model->addSong(s3);

        auto s4 = new Song{this};
        s4->setName(SONG4_NAME);
        s4->setLastPracticed(QDateTime::currentDateTime());
        s4->setProficiency(Song::MediumProficiency);
        m_model->addSong(s4);

        auto s5 = new Song{this};
        s5->setName(SONG5_NAME);
        s5->setLastPracticed(QDateTime::currentDateTime().addDays(-1));
        s5->setProficiency(Song::MediumProficiency);
        m_model->addSong(s5);

        auto s6 = new Song{this};
        s6->setName(SONG6_NAME);
        s6->setLastPracticed(QDateTime::currentDateTime().addDays(-4));
        s6->setProficiency(Song::MediumProficiency);
        m_model->addSong(s6);

        auto s7 = new Song{this};
        s7->setName(SONG7_NAME);
        s7->setLastPracticed(QDateTime::currentDateTime());
        s7->setProficiency(Song::HighProficiency);
        m_model->addSong(s7);

        auto s8 = new Song{this};
        s8->setName(SONG8_NAME);
        s8->setLastPracticed(QDateTime::currentDateTime().addDays(-7));
        s8->setProficiency(Song::HighProficiency);
        m_model->addSong(s8);

        auto s9 = new Song{this};
        s9->setName(SONG9_NAME);
        s9->setLastPracticed(QDateTime::currentDateTime().addDays(-30));
        s9->setProficiency(Song::HighProficiency);
        m_model->addSong(s9);

        m_filter = new SongsFilterModel{m_model};
    }

    void cleanup()
    {
        delete m_filter;
        delete m_model;
    }

    void testFilterOrder()
    {
        QVERIFY(m_filter->rowCount() == 5);

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG6_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG9_NAME);
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG8_NAME);
    }

    void testAddingSongs()
    {
        auto rickroll = new Song{this};
        rickroll->setName("Never Gonna Give You Up");
        rickroll->setLastPracticed(QDateTime::currentDateTime());
        rickroll->setProficiency(Song::HighProficiency);
        m_model->addSong(rickroll);

        testFilterOrder();

        auto rushE = new Song{this};
        rushE->setName("Rush E");
        rushE->setLastPracticed(QDateTime::currentDateTime().addDays(-100));
        rushE->setProficiency(Song::LowProficiency);
        m_model->addSong(rushE);

        QVERIFY(m_filter->rowCount() == 5);

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), "Rush E");
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG6_NAME);
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG9_NAME);
    }

    void testRemovingSongs()
    {
        m_model->removeSong("Hidden: med, today, yesterday");
        testFilterOrder();
    }

private:
    SongsModel *m_model = nullptr;
    SongsFilterModel *m_filter = nullptr;
};

QTEST_MAIN(SongsFilterModelTest)
#include "SongsFilterModelTest.moc"
